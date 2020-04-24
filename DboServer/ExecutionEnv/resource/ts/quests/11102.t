CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11102;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "13;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 13;
			prelnk = "12;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 11065;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "17;15;16;16;14;11;10;9;8;7;6;5;4;3;";

			CDboTSActSendSvrEvt
			{
				id = 11030;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 3;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100822;
				gtype = 0;
				area = 1100807;
				goal = 1100821;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100820;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100814;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 17;
			dt = 0;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "15;";
			dg = 1100938;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 16;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 2;
			prelnk = "17;";
			dg = 1100939;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 15;
			dt = 0;
			lilnk = 255;
			oklnk = 17;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1100937;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 14;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100936;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 7;
			dt = 0;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100815;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "10;";
			dg = 1100819;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1100818;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100817;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 8;
			dt = 0;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "7;";
			dg = 1100816;
			gt = 0;
			oi = 7511102;
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
				id = 11020;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "4;";
			dg = 1100813;
			gt = 0;
			oi = 7511102;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100812;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 2;
			prelnk = "1;";
			dg = 1100811;
			gt = 0;
			oi = 7511102;
		}
	}
}

