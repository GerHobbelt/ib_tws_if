
#include "interthread_comm.h"

#include <mongoose/mongoose_ex.h>




int interthread_communicator::prepare_fd_sets_for_reception(struct fd_set *read_set, struct fd_set *except_set, int &max_fd)
{
	struct socket *sock = mg_get_client_socket(this->outgoing);

	mg_FD_SET(sock, read_set, &max_fd);
	mg_FD_SET(sock, except_set, &max_fd);
	return 0;
}


int interthread_communicator::is_message_pending(fd_set *read_set, fd_set *except_set, int max_fd)
{
	struct socket *sock = mg_get_client_socket(this->outgoing);

	if (mg_FD_ISSET(sock, read_set))
		return 1;
	if (mg_FD_ISSET(sock, except_set))
		return -1;
	return 0;
}

