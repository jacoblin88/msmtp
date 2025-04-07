# Details

Date : 2025-03-23 23:14:51

Directory /home/alex/AI_FUZZ/msmtp

Total : 55 files,  15110 codes, 3751 comments, 1920 blanks, all 20781 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [Dockerfile](/Dockerfile) | Docker | 91 | 24 | 14 | 129 |
| [auth\_fuzzer.c](/auth_fuzzer.c) | C | 103 | 23 | 24 | 150 |
| [docker-compose.yml](/docker-compose.yml) | YAML | 15 | 0 | 2 | 17 |
| [generate\_testcases.sh](/generate_testcases.sh) | Shell Script | 30 | 37 | 31 | 98 |
| [msmtp\_fuzzer.c](/msmtp_fuzzer.c) | C | 95 | 20 | 21 | 136 |
| [msmtpd\_fuzzer.c](/msmtpd_fuzzer.c) | C | 75 | 18 | 19 | 112 |
| [scripts/find\_alias/find\_alias\_for\_msmtp.sh](/scripts/find_alias/find_alias_for_msmtp.sh) | Shell Script | 34 | 17 | 13 | 64 |
| [scripts/msmtpqueue/msmtp-enqueue.sh](/scripts/msmtpqueue/msmtp-enqueue.sh) | Shell Script | 20 | 16 | 9 | 45 |
| [scripts/msmtpqueue/msmtp-listqueue.sh](/scripts/msmtpqueue/msmtp-listqueue.sh) | Shell Script | 5 | 1 | 3 | 9 |
| [scripts/msmtpqueue/msmtp-runqueue.sh](/scripts/msmtpqueue/msmtp-runqueue.sh) | Shell Script | 51 | 8 | 11 | 70 |
| [scripts/set\_sendmail/set\_sendmail.conf](/scripts/set_sendmail/set_sendmail.conf) | Properties | 21 | 40 | 14 | 75 |
| [scripts/set\_sendmail/set\_sendmail.sh](/scripts/set_sendmail/set_sendmail.sh) | Shell Script | 121 | 32 | 33 | 186 |
| [src/aliases.c](/src/aliases.c) | C | 224 | 33 | 48 | 305 |
| [src/aliases.h](/src/aliases.h) | C++ | 10 | 35 | 6 | 51 |
| [src/base64.c](/src/base64.c) | C | 399 | 147 | 60 | 606 |
| [src/base64.h](/src/base64.h) | C++ | 32 | 20 | 17 | 69 |
| [src/conf.c](/src/conf.c) | C | 1,939 | 178 | 114 | 2,231 |
| [src/conf.h](/src/conf.h) | C++ | 121 | 170 | 27 | 318 |
| [src/eval.c](/src/eval.c) | C | 60 | 26 | 13 | 99 |
| [src/eval.h](/src/eval.h) | C++ | 4 | 27 | 4 | 35 |
| [src/gettext.h](/src/gettext.h) | C++ | 218 | 58 | 26 | 302 |
| [src/list.c](/src/list.c) | C | 75 | 45 | 34 | 154 |
| [src/list.h](/src/list.h) | C++ | 16 | 59 | 16 | 91 |
| [src/md5-apps.c](/src/md5-apps.c) | C | 57 | 22 | 15 | 94 |
| [src/md5-apps.h](/src/md5-apps.h) | C++ | 8 | 34 | 6 | 48 |
| [src/md5.c](/src/md5.c) | C | 182 | 69 | 41 | 292 |
| [src/md5.h](/src/md5.h) | C++ | 21 | 27 | 7 | 55 |
| [src/msgid.c](/src/msgid.c) | C | 60 | 27 | 9 | 96 |
| [src/msgid.h](/src/msgid.h) | C++ | 4 | 31 | 4 | 39 |
| [src/msmtp.c](/src/msmtp.c) | C | 3,793 | 361 | 266 | 4,420 |
| [src/msmtpd.c](/src/msmtpd.c) | C | 804 | 82 | 48 | 934 |
| [src/mtls-gnutls.c](/src/mtls-gnutls.c) | C | 595 | 98 | 60 | 753 |
| [src/mtls-libtls.c](/src/mtls-libtls.c) | C | 419 | 81 | 74 | 574 |
| [src/mtls-openssl.c](/src/mtls-openssl.c) | C | 769 | 163 | 91 | 1,023 |
| [src/mtls.c](/src/mtls.c) | C | 148 | 55 | 34 | 237 |
| [src/mtls.h](/src/mtls.h) | C++ | 63 | 165 | 29 | 257 |
| [src/net.c](/src/net.c) | C | 955 | 131 | 118 | 1,204 |
| [src/net.h](/src/net.h) | C++ | 32 | 122 | 17 | 171 |
| [src/netrc.c](/src/netrc.c) | C | 296 | 57 | 72 | 425 |
| [src/netrc.h](/src/netrc.h) | C++ | 29 | 15 | 12 | 56 |
| [src/password.c](/src/password.c) | C | 178 | 32 | 16 | 226 |
| [src/password.h](/src/password.h) | C++ | 11 | 32 | 5 | 48 |
| [src/readbuf.c](/src/readbuf.c) | C | 13 | 31 | 10 | 54 |
| [src/readbuf.h](/src/readbuf.h) | C++ | 11 | 31 | 6 | 48 |
| [src/smtp.c](/src/smtp.c) | C | 1,781 | 310 | 166 | 2,257 |
| [src/smtp.h](/src/smtp.h) | C++ | 120 | 255 | 35 | 410 |
| [src/stream.c](/src/stream.c) | C | 40 | 26 | 10 | 76 |
| [src/stream.h](/src/stream.h) | C++ | 7 | 37 | 7 | 51 |
| [src/tools.c](/src/tools.c) | C | 694 | 165 | 121 | 980 |
| [src/tools.h](/src/tools.h) | C++ | 68 | 173 | 30 | 271 |
| [src/xalloc.c](/src/xalloc.c) | C | 93 | 43 | 25 | 161 |
| [src/xalloc.h](/src/xalloc.h) | C++ | 14 | 26 | 7 | 47 |
| [tests/test-auth-plain.sh](/tests/test-auth-plain.sh) | Shell Script | 14 | 4 | 6 | 24 |
| [tests/test-basic.sh](/tests/test-basic.sh) | Shell Script | 3 | 2 | 3 | 8 |
| [tests/test-header-handling.sh](/tests/test-header-handling.sh) | Shell Script | 69 | 10 | 11 | 90 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)