CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13001;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 623;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "64;6;5;7;7;";

			CDboTSActSendSvrEvt
			{
				id = 624;
				tblidx = 70001;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "64;";
			dg = 1101167;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101165;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 64;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "5;";
			dg = 1101166;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 64;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 1;
			prelnk = "6;";
			dg = 1101224;
			gt = 0;
			oi = -1;
		}
	}
}

