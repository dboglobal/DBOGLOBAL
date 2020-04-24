CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11306;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 1;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100268;
				gtype = -1;
				area = 1100262;
				goal = 1100267;
				m0fz = "-383.220001";
				m0widx = 100000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "831.210022";
				m0ttip = 1100269;
				rwd = 255;
				taid = 1;
				title = 1100266;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 1;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100268;
				gtype = -1;
				area = 1100262;
				goal = 1100267;
				m0fz = "-383.220001";
				m0widx = 193000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "831.210022";
				m0ttip = 1100269;
				rwd = 255;
				taid = 1;
				title = 1100266;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
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
			dg = 1100231;
			gt = 0;
			oi = 7511101;
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
			dg = 1100230;
			gt = 0;
			oi = 7511101;
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
			prelnk = "241;10;8;";
			dg = 1100229;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "0;";
			nolnk = 240;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckInWorld
			{
				widx = 80000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 1;
			prelnk = "9;";

			CDboTSActRegQInfo
			{
				cont = 1100268;
				gtype = -1;
				area = 1100262;
				goal = 1100267;
				m0fz = "-383.220001";
				m0widx = 80000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "831.210022";
				m0ttip = 1100269;
				rwd = 255;
				taid = 1;
				title = 1100266;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActSendSvrEvt
			{
				id = 13071;
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

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;2;3;4;4;";

			CDboTSActSendSvrEvt
			{
				id = 13060;
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
			cid = 240;
			prelnk = "9;";
			nolnk = 10;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 193000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 9;

			CDboTSRcvSvrEvt
			{
				id = 13050;
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
			dg = 1100232;
			gt = 0;
			oi = 7511101;
		}
	}
}

