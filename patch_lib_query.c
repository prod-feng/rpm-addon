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
