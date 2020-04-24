CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11418;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;2;1;3;";

			CDboTSActSendSvrEvt
			{
				id = 14410;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100340;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "0;";
			dg = 1100339;
			gt = 0;
			oi = 3511102;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "2;";
			dg = 1100341;
			gt = 0;
			oi = 3511102;
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
				id = 14400;
			}
		}
	}
}

