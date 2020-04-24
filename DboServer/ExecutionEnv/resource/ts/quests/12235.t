CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12235;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;7;6;5;4;3;2;1;8;";

			CDboTSActSendSvrEvt
			{
				id = 1066;
				tblidx = -1;
				stype = 2;
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
			os = 2;
			prelnk = "7;";
			dg = 1101080;
			gt = 0;
			oi = 1151102;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "11;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 254;
			prelnk = "10;10;";

			CDboTSActSendSvrEvt
			{
				id = 1067;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
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
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1101081;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1101077;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "5;";
			dg = 1101078;
			gt = 0;
			oi = 1151102;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1101079;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "3;";
			dg = 1101076;
			gt = 0;
			oi = 1151102;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101073;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "1;";
			dg = 1101074;
			gt = 0;
			oi = 1151102;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101075;
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
				id = 1065;
			}
		}
	}
}

