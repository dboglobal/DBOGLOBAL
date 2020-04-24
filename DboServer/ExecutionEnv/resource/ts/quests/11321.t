CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11321;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "1;2;3;3;";

			CDboTSActSendSvrEvt
			{
				id = 13422;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 13420;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 13440;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 1;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100254;
			gt = 0;
			oi = 1612101;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100253;
			gt = 0;
			oi = 1612101;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 1;
			dt = 0;
			lilnk = 255;
			oklnk = 2;
			ot = 1;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100252;
			gt = 0;
			oi = 1612101;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 13421;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 420000;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5681200;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100285;
				gtype = -1;
				area = 1100262;
				goal = 1100284;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100283;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

