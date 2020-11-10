/**
 * Copyright (c) 2007-2012, Timothy Stack
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "auto_fd.hh"

int main(int argc, char *argv[])
{
    int retval = EXIT_SUCCESS;
    auto_fd fd1, fd2;
    int tmp;
    
    assert(fd1 == -1);
    tmp = open("/dev/null", O_RDONLY);
    assert(tmp != -1);
    fd1 = tmp;
    fd1 = tmp;
    assert(fcntl(tmp, F_GETFL) >= 0);
    fd1 = std::move(fd2);
    assert(fcntl(tmp, F_GETFL) == -1);
    assert(errno == EBADF);
    assert(fd1 == -1);

    tmp = open("/dev/null", O_RDONLY);
    assert(tmp != -1);
    fd1 = tmp;
    *fd1.out() = STDOUT_FILENO;
    assert(fcntl(tmp, F_GETFL) == -1);
    assert(errno == EBADF);

    {
	auto_fd fd_cp(const_cast<const auto_fd &>(fd1));

	assert(fd1 == STDOUT_FILENO);
	assert(fd_cp != STDOUT_FILENO);
	assert(fd_cp != -1);

	tmp = (int)fd_cp;
    }
    {
	auto_fd fd_cp(const_cast<const auto_fd &>(fd1));

	assert(fd_cp == tmp);
    }
    assert(fd1.release() == STDOUT_FILENO);
    assert(fd1 == -1);
    
    return retval;
}
