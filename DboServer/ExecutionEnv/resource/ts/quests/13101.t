CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13101;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 202;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 201;
			prelnk = "11;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 201;
			prelnk = "10;9;8;10;";

			CDboTSActSendSvrEvt
			{
				id = 646;
				tblidx = 70100;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 202;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 643;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1101252;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1101251;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1101250;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 645;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;5;4;3;2;1;";

			CDboTSActSendSvrEvt
			{
				id = 644;
				tblidx = 70100;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1101249;
			gt = 0;
			oi = 1331201;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1101248;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101247;
			gt = 0;
			oi = 1331201;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1101246;
			gt = 0;
			oi = -1;
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
			os = 0;
			prelnk = "0;";
			dg = 1101245;
			gt = 0;
			oi = 1331201;
		}
	}
}

