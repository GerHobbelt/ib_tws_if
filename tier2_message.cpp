
// Copyright (c) 2011 Ger Hobbelt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */

#include "tier2_message.h"

#include "app_manager.h"
#include "tier2_message_processor.h"
#include "interthread_comm.h"

#include <mongoose/mongoose_ex.h>





unique_type_id_threadsafe_manager tier2_msg_typeid_mgr;




tier2_message::state_change tier2_message::handle_state_change(tier2_message::request_state_t new_state)
{
    state_change rv = PROCEED;
    /*
    we copy the handler list before traversing it as the list may change as a 'side effect'
    of invoking the handler->process() method!
    */
    state_change_handler_set_t h_set(m_state_change_handlers);

    for (size_t i = 0; i < h_set.size(); i++)
    {
        tier2_message_state_change_handler *h = h_set[i];

        assert(h);
        switch (h->process(*this, new_state))
        {
        default:
        case ERROR_OCCURRED:
            rv = ERROR_OCCURRED;
            break;

        case DONT_CHANGE:
            rv = DONT_CHANGE;
            break;

        case PROCEED:
            continue;
        }
        break;
    }
    return rv;
}




void tier2_message::register_handler(tier2_message_state_change_handler *handler)
{
    for (state_change_handler_set_t::iterator i = m_state_change_handlers.begin();
        i != m_state_change_handlers.end();
        i++)
    {
        tier2_message_state_change_handler *h = *i;

        if (h == handler)
            return;
    }
    m_state_change_handlers.push_back(handler);
}

void tier2_message::unregister_handler(tier2_message_state_change_handler *handler)
{
    for (state_change_handler_set_t::iterator i = m_state_change_handlers.begin();
        i != m_state_change_handlers.end();
        i++)
    {
        tier2_message_state_change_handler *h = *i;

        if (h == handler)
        {
            m_state_change_handlers.erase(i);
            return;
        }
    }
}






// set up the defaults; perform any necessary registration with the app_manager, etc...
void tier2_message::resolve_requester_and_receiver_issues(void)
{
    assert(requester || m_receiver);

    /*
    Default: receiver == requester.
    */
    if (!m_receiver)
    {
        assert(dynamic_cast<tier2_message_processor *>(requester));
        m_receiver = requester;
        assert(m_receiver);
    }
    if (!requester)
    {
        requester = m_receiver;
    }

    if (requester != m_receiver)
    {
        /*
        Also register the thread interconnect so that we can re-use it
        later on to send/receive additional messages across the same
        interconnection.

        The creation/registration is a side effect of the query:
        */
        (void)requester->get_interthread_communicator(requester, m_receiver);
    }

    assert(!m_owner);
    current_owner(requester);
}












tier2_message::tier2_message(tier2_message_processor *from, tier2_message_processor *to, request_state_t initial_state) :
    requester(from),
    m_receiver(to),
    m_now_state(initial_state),
    m_previous_state(INIT4PREV),
    m_owner(NULL)
{
    resolve_requester_and_receiver_issues();
}

tier2_message::~tier2_message()
{
    // state(DESTRUCTION); -- can't do that here as derived classes will already have destructed themselves! Hence protected destructor!
    
    //m_owner = NULL;
    //current_owner(m_owner);
}

tier2_message::request_state_t tier2_message::state(request_state_t new_state)
{
    for (int i = 2; i > 0 && new_state != m_now_state; i--)
    {
        new_state = exec_state(new_state);
    }
    return new_state;
}

tier2_message::request_state_t tier2_message::exec_state(request_state_t new_state)
{
    tier2_message::state_change rv = handle_state_change(new_state);

    switch (rv)
    {
    default:
    case ERROR_OCCURRED:
        if (m_previous_state != m_now_state)
            m_previous_state = m_now_state;
        m_now_state = new_state = FAILED;
        break;

    case DONT_CHANGE:
        new_state = m_now_state;
        // do NOT execute the f_* state handlers for the same state again!
        return new_state;

    case PROCEED:
        if (m_previous_state != m_now_state)
            m_previous_state = m_now_state;
        m_now_state = new_state;
        break;
    }

    assert(m_owner);
    {
        int err = 0;

        switch (new_state)
        {
        case EXEC_COMMAND:
            // receiver ~ handler
            if (m_owner == m_receiver)
            {
                err = f_exec_command();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case WAIT_FOR_TRANSMIT:
            // receiver ~ handler
            if (m_owner == m_receiver)
            {
                err = f_wait_for_transmit();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case COMMENCE_TRANSMIT:
            // receiver ~ handler
            if (m_owner == m_receiver)
            {
                err = f_commence_transmit();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case READY_TO_RECEIVE_RESPONSE:
            // receiver ~ handler
            if (m_owner == m_receiver)
            {
                err = f_ready_to_receive_response();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case RESPONSE_PENDING:
            // receiver ~ handler
            if (m_owner == m_receiver)
            {
                err = f_response_pending();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case RESPONSE_COMPLETE:
            // requester ~ handler
            if (m_owner == requester)
            {
                err = f_response_complete();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case TASK_COMPLETED:
            // requester ~ handler
            if (m_owner == requester)
            {
                err = f_task_completed();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case tier2_message::DESTRUCTION:                // T: just before the destructor is invoked: last call!
            // requester ~ handler
            if (m_owner == requester)
            {
                err = f_destruction_imminent();
            }
            /*
            'destruction' itself is also a 'pulsable' task so ALWAYS queue the bugger for pulsing.

             We ignore the case where the f_destruction_imminent() function would've changed
             the state on us; the 'pulsing' process is smart enough to cope with such
             'nothing to do here' occasions as may occur then.
            */
            m_owner->queue_msg_for_pulsing(this);
            break;

        case tier2_message::FAILED:                     // T: when an error occurred
            // requester ~ handler
            if (m_owner == requester)
            {
                err = f_task_failed();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        case tier2_message::ABORTED:                    // T: when the request has been canceled
            // requester ~ handler
            if (m_owner == requester)
            {
                err = f_task_aborted();
            }
            else
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;

        default:
            if (m_owner != requester)
            {
                m_owner->queue_msg_for_pulsing(this);
            }
            break;
        }

        /*
        WARNING: the f_*() template functions may have bumped the state to a new value;
                 we should be aware of that possibility and NOT revert the state in
                 our own outer state() loop:
        */
        new_state = m_now_state;

        // changing to FAILED state is only useful when we're still in charge here, otherwise it's only cause to collisions!
        assert(m_owner);
        if (err != 0)
        {
            new_state = FAILED;
        }
    }

    return new_state;
}

int tier2_message::pulse(void)
{
    interthread_communicator *comm = NULL;
    int err = 0;

    assert(m_owner);

    switch (m_now_state)
    {
    case DESTRUCTION:               // T: just before the destructor is invoked: last call!
        // send message to receiver ~ handler
        if (m_owner != requester)
        {
            comm = m_owner->get_interthread_communicator(m_owner, requester);
            assert(comm);

            // push message across the pond:
            err = comm->post_message(this);
        }
        else
        {
            m_owner->release(this);
            m_owner = NULL;

            delete this;
        }
        break;

    case EXEC_COMMAND:
    case WAIT_FOR_TRANSMIT:
    case COMMENCE_TRANSMIT:
    case READY_TO_RECEIVE_RESPONSE:
    case RESPONSE_PENDING:
        // send message to receiver ~ handler
        if (m_owner != m_receiver)
        {
            comm = m_owner->get_interthread_communicator(m_owner, m_receiver);
            assert(comm);

            // push message across the pond:
            err = comm->post_message(this);
        }
        break;

    case RESPONSE_COMPLETE:
    case TASK_COMPLETED:
    case FAILED:                    // T: when an error occurred
    case ABORTED:                   // T: when the request has been canceled
    default:
        // send message to requester ~ handler
        if (m_owner != requester)
        {
            comm = m_owner->get_interthread_communicator(m_owner, requester);
            assert(comm);

            // push message across the pond:
            err = comm->post_message(this);
        }
        break;
    }
    return err;
}

/*
Set the new owner.

A notable side effect is that the message state handlers/observers (registered visitors)
are immediately notified about this change by invoking them.
*/
tier2_message_processor *tier2_message::current_owner(tier2_message_processor *new_owner)
{
    if (m_owner != new_owner)
    {
        /*
        message should be 'in limbo' when transferring between owners. If the message is not,
        it would mean that a

            m_owner->release(this);

        call would be required AND that the invoked release() method would require thread
        safety measures (mutex or other).
        */
        assert(!m_owner || !new_owner);
        if (m_owner)
        {
            m_owner->release(this);
            m_owner = NULL;
        }
        if (new_owner)
        {
            new_owner->own(this);

            // and re-run the state processing again so the state-related task can be performed:
            m_owner = new_owner; // set up an owner for the state handlers
            request_state_t new_state = exec_state(m_now_state);
            if (new_state != m_now_state)
            {
                exec_state(new_state);
            }
        }
    }

    return m_owner;
}













int tier2_message::cancel_request(tier2_message_processor *transmitter)
{
    // create a CANCEL message:
    cancel_message *cancel = new cancel_message(transmitter, this);

    // push message across the pond:
    cancel->state(EXEC_COMMAND);

    return 0;
}

interthread_communicator *tier2_message::get_interthread_communicator(bool tx2rx)
{
    assert(m_owner);
    if (requester != m_receiver)
    {
        if (tx2rx)
            return m_owner->get_interthread_communicator(requester, m_receiver);
        else
            return m_owner->get_interthread_communicator(m_receiver, requester);
    }
    return NULL;
}

int tier2_message::wait_for_response(interthread_communicator *listen_comm)
{
    assert(requester != m_receiver);
    //interthread_communicator *comm = m_owner->get_interthread_communicator(m_receiver, requester);  -- m_owner is in limbo, hence untrustworthy!
    assert(listen_comm);
    assert(listen_comm->receiver() == requester);

    if (!listen_comm)
        return -1;

    // check the message queue of the requester to see if the expected message already was returned before we called this!
    if (requester->does_own(this))
    {
        switch(this->state())
        {
        case MSG_INITIALIZED:
        case EXEC_COMMAND:
        case WAIT_FOR_TRANSMIT:
        case COMMENCE_TRANSMIT:
        case READY_TO_RECEIVE_RESPONSE:
        case RESPONSE_PENDING:
            break;

        case RESPONSE_COMPLETE:
        case TASK_COMPLETED:
        default:
            // our message has already been answered / completed / aborted ... it's concluded anyway.
            return 0;
        }
    }

    while (mg_get_stop_flag(mg_get_context(listen_comm->receiver_socket())) == 0)
    {
        // make me wait for a message to arrive...
        interthread_communicator::msg_pending_mode_t msg_mode = interthread_communicator::NO_MSG;

        tier2_message *msg = listen_comm->pop_one_message(msg_mode);

        // if the popped message is our response (and not some book-keeping sort of thing), we know we're done.
        if (!msg)
        {
            return -1;
        }
        if (msg == this)
        {
            return 0;
        }
    }
    return -1;
}

int tier2_message::process_response_message(tier2_message *resp_msg)
{
    assert(!"Should never get here!");
    return 0;
}




















int tier2_message::f_exec_command(void)
{
    assert(!"Should never get here");
    return 0;
}
int tier2_message::f_task_completed(void)
{
    return 0;
}
int tier2_message::f_response_complete(void)
{
    return 0;
}
int tier2_message::f_response_pending(void)
{
    return 0;
}
int tier2_message::f_ready_to_receive_response(void)
{
    return 0;
}
int tier2_message::f_commence_transmit(void)
{
    return 0;
}
int tier2_message::f_wait_for_transmit(void)
{
    state(tier2_message::COMMENCE_TRANSMIT);

    return 0;
}
int tier2_message::f_task_failed(void)
{
    return 0;
}
int tier2_message::f_task_aborted(void)
{
    return 0;
}
int tier2_message::f_destruction_imminent(void)
{
    return 0;
}









int cancel_message::f_exec_command(void)
{
    assert(m_refd_msg);
    assert(m_owner == m_receiver);

    /*
    prevent race condition where original msg already got sent back to the requester while a cancel request (this one) was still pending in the receiver queue:

    --> check if referenced message is still owned by our owner/its target.

    When it isn't, the target is either 'in limbo' or already completely processed and destroyed. We're trying to prevent illegal memory access core dumps due to the
    latter, while we decided not to bother treating the former as a special case: when the target IS 'in limbo', it means the target message has probably already
    completed and is returning to the requester; meanwhile it ALSO means that the target's STATE isn't anymore what we think it is and that clashes a wee bit
    with the expectation when the CANCEL was fired.

    The way out of this conundrum about the 'in limbo' ambiguity (has the targeted message arrived yet or has it already left again?) is to have a non-applicable
    cancel request remain 'active' until the incoming (receive) queue of the targeted processor is empty: when that happens we are certain that the targeted
    message has truly already left again as it has been sent to this processor before we ('the cancel request') got sent there, so it should have popped up in
    the queue a little while after we arrived, at the very latest.
    */
    if (m_receiver->does_own(m_refd_msg))
    {
        tier2_message::request_state_t rv = m_refd_msg->state(tier2_message::ABORTED);

        return rv == tier2_message::ABORTED;
    }
    else
    {
        // TODO: push cancel message in the active cancel set for the receiver/processor.
    }
    return -1;
}












