CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11201;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActNPCConv
			{
				conv = 1100468;
				ctype = 1;
				idx = 5551103;
				taid = 3;
			}
			CDboTSActNPCConv
			{
				conv = 1100467;
				ctype = 1;
				idx = 2761102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 1100181;
				gtype = 1;
				area = 1100179;
				goal = 1100180;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100178;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 7;
			dt = 0;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "6;";
			dg = 1100163;
			gt = 0;
			oi = 2761102;
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100162;
			gt = 0;
			oi = 5551103;
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "4;";
			dg = 1100161;
			gt = 0;
			oi = 2761102;
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100160;
			gt = 0;
			oi = 5551103;
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100159;
			gt = 0;
			oi = 2761102;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;4;5;6;7;7;";

			CDboTSActSendSvrEvt
			{
				id = 12060;
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
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSRcvSvrEvt
			{
				id = 12050;
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
				id = 12020;
			}
		}
	}
}

