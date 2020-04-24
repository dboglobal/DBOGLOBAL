CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11400;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;8;8;";

			CDboTSActSendSvrEvt
			{
				id = 14030;
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
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 14020;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;2;3;4;4;";

			CDboTSActSendSvrEvt
			{
				id = 14010;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100300;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100299;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100295;
			gt = 0;
			oi = 7511101;
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
				id = 14000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100298;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100297;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100296;
			gt = 0;
			oi = 2413109;
		}
	}
}

