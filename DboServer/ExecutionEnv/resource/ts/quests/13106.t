CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13106;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 222;
			prelnk = "20;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 20;
			elnk = 255;
			nextlnk = 222;
			prelnk = "19;18;17;19;";

			CDboTSActSendSvrEvt
			{
				id = 667;
				tblidx = 70120;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 16;
			prelnk = "15;";
			nolnk = 255;
			rm = 0;
			yeslnk = 17;

			CDboTSRcvSvrEvt
			{
				id = 665;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "14;14;13;12;11;10;9;8;7;";

			CDboTSActSendSvrEvt
			{
				id = 664;
				tblidx = 70120;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;3;2;1;4;";

			CDboTSActSendSvrEvt
			{
				id = 662;
				tblidx = 70120;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 20;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "18;";
			dg = 1101279;
			gt = 0;
			oi = 3301201;
		}
		CDboTSContNarration
		{
			cancellnk = 20;
			cid = 18;
			dt = 2;
			lilnk = 255;
			oklnk = 19;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "17;";
			dg = 1101278;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 20;
			cid = 17;
			dt = 2;
			lilnk = 255;
			oklnk = 18;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "16;";
			dg = 1101277;
			gt = 0;
			oi = 3301201;
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
				id = 663;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1101270;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1101276;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 13;
			dt = 2;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "12;";
			dg = 1101275;
			gt = 0;
			oi = 3301201;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 12;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1101274;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 12;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1101273;
			gt = 0;
			oi = 3301201;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1101272;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1101271;
			gt = 0;
			oi = 3301201;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1101269;
			gt = 0;
			oi = 3301201;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 661;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1101267;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101266;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1101265;
			gt = 0;
			oi = -1;
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
			dg = 1101264;
			gt = 0;
			oi = 7611209;
		}
	}
}

