CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11123;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "14;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;13;12;11;8;9;10;7;6;5;";

			CDboTSActSendSvrEvt
			{
				id = 11320;
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
			nextlnk = 4;
			prelnk = "0;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7111400;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100894;
				gtype = 1;
				area = 1100807;
				goal = 1100893;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100892;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100896;
			gt = 0;
			oi = 4612101;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 11310;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 13;
			dt = 0;
			lilnk = 255;
			oklnk = 14;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "12;";
			dg = 1100903;
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
			cancellnk = 14;
			cid = 12;
			dt = 0;
			lilnk = 255;
			oklnk = 13;
			ot = 1;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1100902;
			gt = 0;
			oi = 4612101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100901;
			gt = 0;
			oi = 7111404;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100900;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100899;
			gt = 0;
			oi = 4612101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 8;
			dt = 0;
			lilnk = 255;
			oklnk = 9;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100898;
			gt = 0;
			oi = 4612101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 7;
			dt = 0;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100897;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 1;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100895;
			gt = 0;
			oi = 7111404;
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
				id = 11275;
			}
		}
	}
}

