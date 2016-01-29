# rpm-addon
Fedora 10, RPM package management, enhance "whatrequires" option ,coding, C

Solution for non-accurate dependency information by query with --whatrequires option

rpm (version 4.8.1) cannot provide fully accurate dependency information when query with --whatrequires option. July, 2010.

Symptom of the problem:

 -----------------------------------------------------------------------------

 [abc@localhost rpm-4.8.1]$ /bin/rpm -q --whatrequires wireless-tools
 
          no package requires wireless-tools

 -----------------------------------------------------------------------------
 
 [abc@localhost rpm-4.8.1]$ /bin/rpm -e wireless-tools
 
          error: Failed dependencies:
 
         libiw.so.29 is needed by (installed) rhpl-0.218-1.i386

 -----------------------------------------------------------------------------

 The query action cannot see any packages that require wireless-tools,
 while when try to remove this "wireless-tools" package, rpm can find
 correctly that "rhpl" requires it.

 The expected behavior for rpm is that the above two methods should have
 the same results.

 Try to update the source file of lib/query.c as follows:
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     ...

     Header h;  //NB. declare in the beginning of the function

     ...

     case RPMQV_WHATREQUIRES:
         //

         qva->qva_mi = rpmtsInitIterator(ts, RPMDBI_LABEL, arg, 0);
         Header h;
             while ((h = rpmdbNextIterator(qva->qva_mi)) != NULL) {
                     (void) rpmtsAddEraseElement(ts, h, -1);
             }

         if (rpmtsCheck(ts)) {
         }
         ps = rpmtsProblems(ts);

         qva->qva_mi = rpmtsInitIterator(ts, RPMTAG_REQUIRENAME, arg, 0);

         if (qva->qva_mi == NULL&&rpmpsNumProblems(ps)<=0) {
             rpmlog(RPMLOG_NOTICE, _("no package requires %s\n"), arg);
             res = 1;
         } else
             res = rpmcliShowMatches(qva, ts);

             rpmpsPrint(NULL, ps);
         break;

 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


 The updated codes try to use the method in "Erase" to check the dependency
 of the package.

 The new results look more reasonable as follows(while there might be other
 ways to implement this better...):

 --------------------------------------------------------------------------------

 [abc@localhost rpm-4.8.1]$ /usr/local/bin/rpm --dbpath  /usr/local/var/lib/rpm/ -q --whatrequires wireless-tools
 
         libiw.so.29 is needed by (installed) rhpl-0.218-1.i386

 --------------------------------------------------------------------------------

 [abc@localhost rpm-4.8.1]$ /usr/local/bin/rpm --dbpath  /usr/local/var/lib/rpm/ -e wireless-tools
 
         error: Failed dependencies:
 
         libiw.so.29 is needed by (installed) rhpl-0.218-1.i386

 --------------------------------------------------------------------------------
