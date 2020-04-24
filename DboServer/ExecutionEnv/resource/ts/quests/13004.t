CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13004;
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
			yeslnk = 21;

			CDboTSRcvSvrEvt
			{
				id = 630;
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
			prelnk = "27;26;25;24;23;22;21;28;28;";

			CDboTSActSendSvrEvt
			{
				id = 631;
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
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 21;
			dt = 2;
			lilnk = 255;
			oklnk = 22;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101181;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 28;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "27;";
			dg = 1101188;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 22;
			dt = 2;
			lilnk = 255;
			oklnk = 23;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "21;";
			dg = 1101182;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 23;
			dt = 2;
			lilnk = 255;
			oklnk = 24;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "22;";
			dg = 1101183;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 27;
			dt = 2;
			lilnk = 255;
			oklnk = 28;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "26;";
			dg = 1101187;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 24;
			dt = 2;
			lilnk = 255;
			oklnk = 25;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "23;";
			dg = 1101184;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 25;
			dt = 2;
			lilnk = 255;
			oklnk = 26;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "24;";
			dg = 1101185;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 26;
			dt = 2;
			lilnk = 255;
			oklnk = 27;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "25;";
			dg = 1101186;
			gt = 0;
			oi = 1381101;
		}
	}
}

