CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11603;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 4;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 11603;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100460;
				gtype = -1;
				area = 1100407;
				goal = 1100459;
				m0fz = "-1002.780029";
				m0widx = 160000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100458;
				rwd = 255;
				taid = 2;
				title = 1100458;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "1;";
			nolnk = 6;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 150000;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 5;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckInWorld
			{
				widx = 120000;
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
				id = 16010;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 11603;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100460;
				gtype = -1;
				area = 1100407;
				goal = 1100459;
				m0fz = "-1002.780029";
				m0widx = 150000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100458;
				rwd = 255;
				taid = 2;
				title = 1100458;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "6;3;2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 16060;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActWorldPlayScript
			{
				sid = 11603;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100460;
				gtype = -1;
				area = 1100407;
				goal = 1100459;
				m0fz = "-1002.780029";
				m0widx = 120000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100458;
				rwd = 255;
				taid = 2;
				title = 1100458;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
	}
}

