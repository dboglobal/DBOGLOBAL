CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13008;
	title = 1300002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "41;41;40;";

			CDboTSActSendSvrEvt
			{
				id = 678;
				tblidx = 70004;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 40;

			CDboTSRcvSvrEvt
			{
				id = 677;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 40;
			dt = 2;
			lilnk = 255;
			oklnk = 41;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101200;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 41;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 2;
			prelnk = "40;";
			dg = 1101201;
			gt = 0;
			oi = 1381101;
		}
	}
}

