CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13011;
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
			prelnk = "45;46;47;48;49;50;51;52;53;54;55;56;57;57;";

			CDboTSActSendSvrEvt
			{
				id = 684;
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
			yeslnk = 45;

			CDboTSRcvSvrEvt
			{
				id = 683;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 57;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "56;";
			dg = 1101217;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 45;
			dt = 2;
			lilnk = 255;
			oklnk = 46;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101205;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 46;
			dt = 2;
			lilnk = 255;
			oklnk = 47;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "45;";
			dg = 1101206;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 47;
			dt = 2;
			lilnk = 255;
			oklnk = 48;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "46;";
			dg = 1101207;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 48;
			dt = 2;
			lilnk = 255;
			oklnk = 49;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "47;";
			dg = 1101208;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 49;
			dt = 2;
			lilnk = 255;
			oklnk = 50;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "48;";
			dg = 1101209;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 51;
			cid = 50;
			dt = 2;
			lilnk = 255;
			oklnk = 51;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "49;";
			dg = 1101210;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 51;
			dt = 2;
			lilnk = 255;
			oklnk = 52;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "50;50;";
			dg = 1101211;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 52;
			dt = 2;
			lilnk = 255;
			oklnk = 53;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "51;";
			dg = 1101212;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 53;
			dt = 2;
			lilnk = 255;
			oklnk = 54;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "52;";
			dg = 1101213;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 56;
			dt = 2;
			lilnk = 255;
			oklnk = 57;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "55;";
			dg = 1101216;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 55;
			dt = 2;
			lilnk = 255;
			oklnk = 56;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "54;";
			dg = 1101215;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 54;
			dt = 2;
			lilnk = 255;
			oklnk = 55;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "53;";
			dg = 1101214;
			gt = 0;
			oi = 1381101;
		}
	}
}

