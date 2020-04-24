CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11101;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 11015;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 1100810;
				ctype = 1;
				idx = 7511102;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100809;
				gtype = 1;
				area = 1100807;
				goal = 1100808;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100806;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100805;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100804;
			gt = 0;
			oi = 7111401;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "1;";
			dg = 1100803;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;3;2;4;";

			CDboTSActSendSvrEvt
			{
				id = 11010;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
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
				id = 11000;
			}
		}
	}
}

