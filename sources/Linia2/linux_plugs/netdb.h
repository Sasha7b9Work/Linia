#pragma once

int getnameinfo(socklen_t hostlen, socklen_t servlen;
                const struct sockaddr *restrict addr, socklen_t addrlen,
                char host[_Nullable restrict hostlen],
                socklen_t hostlen,
                char serv[_Nullable restrict servlen],
                socklen_t servlen,
                int flags);
                       