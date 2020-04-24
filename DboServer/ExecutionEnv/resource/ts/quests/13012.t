CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13012;
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
			prelnk = "63;63;62;61;60;59;58;";

			CDboTSActSendSvrEvt
			{
				id = 686;
				tblidx = 70004;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 58;

			CDboTSRcvSvrEvt
			{
				id = 685;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 58;
			dt = 2;
			lilnk = 255;
			oklnk = 59;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101218;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 63;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "62;";
			dg = 1101223;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 59;
			dt = 2;
			lilnk = 255;
			oklnk = 60;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "58;";
			dg = 1101219;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 61;
			dt = 2;
			lilnk = 255;
			oklnk = 62;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "60;";
			dg = 1101221;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 60;
			dt = 2;
			lilnk = 255;
			oklnk = 61;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "59;";
			dg = 1101220;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 62;
			dt = 2;
			lilnk = 255;
			oklnk = 63;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "61;";
			dg = 1101222;
			gt = 0;
			oi = 1381101;
		}
	}
}

