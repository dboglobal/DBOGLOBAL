CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11602;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "8;";

			CDboTSActWorldPlayScript
			{
				sid = 11602;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 6;
			prelnk = "10;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8151100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100454;
				gtype = -1;
				area = 1100407;
				goal = 1100453;
				m0fz = "-1009.010010";
				m0widx = 160000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "869.640015";
				m0ttip = 1100452;
				rwd = 255;
				taid = 2;
				title = 1100452;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;2;";
			nolnk = 10;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckInWorld
			{
				widx = 120000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "11;5;4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "3;";
			nolnk = 11;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckInWorld
			{
				widx = 150000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActRegQInfo
			{
				cont = 1100454;
				gtype = -1;
				area = 1100407;
				goal = 1100453;
				m0fz = "-1009.010010";
				m0widx = 120000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "869.640015";
				m0ttip = 1100452;
				rwd = 255;
				taid = 2;
				title = 1100452;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8151100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
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
				id = 16000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "10;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8151100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100454;
				gtype = -1;
				area = 1100407;
				goal = 1100453;
				m0fz = "-1009.010010";
				m0widx = 150000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "869.640015";
				m0ttip = 1100452;
				rwd = 255;
				taid = 2;
				title = 1100452;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 1;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100384;
			gt = 0;
			oi = 8151101;
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActSendSvrEvt
			{
				id = 16010;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActWorldPlayScript
			{
				sid = 11602;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;";
			type = 1;
		}
	}
}

