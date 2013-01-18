#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int verbose = 0;
int v4ok = 1;
int v6ok = 1;
int printall = 0;

int
usage()
{
  printf("getaddrinfo [-hv46a] <addr>...\n");
  printf("    -h : print help\n");
  printf("    -v : verbose\n");
  printf("    -4 : IPv4 only\n");
  printf("    -6 : IPv6 only\n");
  printf("    -a : print all\n");
  return 1;
}

int
do_getaddr(const char *host)
{
  struct addrinfo hints;
  struct addrinfo *res0 = NULL;
  memset((void *)&hints, 0, sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  int error = getaddrinfo(host, "ftp", &hints, &res0);
  if (error) {
    errx(1, "%s", gai_strerror(error));
    /*NOTREACHED*/
  }

  if (verbose)
    printf("%s :", host);

  int result = 0;
  int data_put = 0;
  for (struct addrinfo *res=res0; res!=NULL; res=res->ai_next) {
    if (! (v4ok && res->ai_family==AF_INET)
	&& ! (v6ok && res->ai_family==AF_INET6)) {
      continue;
    }

    char hbuf[NI_MAXHOST + 16];
    char sbuf[NI_MAXSERV + 16];
    int error = getnameinfo(
      (struct sockaddr *)res->ai_addr, res->ai_addrlen,
      hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST);
    if (error) {
      fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(error));
      result = 1;
      continue;
    }
    if (verbose || data_put)
      printf(" ");
    printf("%s", hbuf);
    data_put = 1;
    if (! printall)
      break;
  }
  freeaddrinfo(res0);

  if (verbose || data_put>0)
    printf("\n");

  return result;
}

int
main(int argc, char **argv)
{
  for (int ch=':'; ch!=-1; ch=getopt(argc, argv, "hv46a")) {
    switch (ch) {
    case ':':
      // do nothing
      break;
    case 'v':
      verbose ++;
      break;
    case '4':
      v4ok = 1;
      v6ok = 0;
      break;
    case '6':
      v4ok = 0;
      v6ok = 1;
      break;
    case 'a':
      printall = 1;
      break;
    case '?':
    case 'h':
    default:
      return usage();
    }
  }
  argc -= optind;
  argv += optind;

  int result = 0;
  for (int i=0; i<argc; i++) {
    if (do_getaddr(argv[i]))
      result = 1;
  }
  return result;
}
