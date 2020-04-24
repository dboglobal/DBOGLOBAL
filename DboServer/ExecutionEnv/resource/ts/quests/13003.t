CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13003;
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
			yeslnk = 14;

			CDboTSRcvSvrEvt
			{
				id = 627;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "15;14;16;16;";

			CDboTSActSendSvrEvt
			{
				id = 628;
				tblidx = 70002;
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
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 16;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "15;";
			dg = 1101176;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 15;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1101175;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101174;
			gt = 0;
			oi = 1381101;
		}
	}
}

