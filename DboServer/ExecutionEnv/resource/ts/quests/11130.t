CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11130;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 240;
			prelnk = "6;";
			nolnk = 1;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 5;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100932;
				gtype = 0;
				area = 1100807;
				goal = 1100931;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100930;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 4612100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112416;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 240;
			prelnk = "0;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100857;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "241;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSRcvSvrEvt
			{
				id = 11430;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 11460;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 11440;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 1;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100923;
			gt = 0;
			oi = 4612101;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 5;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100932;
				gtype = 0;
				area = 1100807;
				goal = 1100931;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100930;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 4612100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112410;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

