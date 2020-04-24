CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11320;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 13415;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;1;";

			CDboTSActSendSvrEvt
			{
				id = 13390;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100282;
				gtype = -1;
				area = 1100262;
				goal = 1100281;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100280;
			}
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
				id = 13380;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100247;
			gt = 0;
			oi = 7511101;
		}
	}
}

