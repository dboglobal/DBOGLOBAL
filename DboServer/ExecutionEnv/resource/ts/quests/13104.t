CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13104;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 201;
			prelnk = "10;9;8;10;";

			CDboTSActSendSvrEvt
			{
				id = 655;
				tblidx = 70110;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1560;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;4;3;2;1;";

			CDboTSActSendSvrEvt
			{
				id = 653;
				tblidx = 70110;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 201;
			prelnk = "11;";
			type = 1;
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
				id = 654;
			}
		}
		CDboTSContEnd
		{
			cid = 200;
			prelnk = "0;";
			type = 0;
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
			dg = 1101263;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1101262;
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
			dg = 1101261;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 200;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 652;
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
			dg = 1101260;
			gt = 0;
			oi = 7581101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101259;
			gt = 0;
			oi = -1;
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
			dg = 1101258;
			gt = 0;
			oi = 7611209;
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
			dg = 1101257;
			gt = 0;
			oi = 7581101;
		}
	}
}

