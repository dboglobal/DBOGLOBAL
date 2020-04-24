CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12201;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 12205;
				start = 0;
				taid = 1;
				uof = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 22070;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 253;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 12205;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 22050;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 253;
			nextlnk = 1;
			prelnk = "0;";

			CDboTSActWorldPlayScript
			{
				sid = 12212;
				start = 1;
				taid = 2;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1101108;
				gtype = -1;
				area = 1101104;
				goal = 1101107;
				grade = -1;
				rwd = 255;
				sort = -1;
				stype = 4;
				taid = 1;
				title = 1101106;
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
				sid = 12212;
				start = 0;
				taid = 2;
				uof = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 22040;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSRcvSvrEvt
			{
				id = 600;
			}
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
				id = 22370;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "3;5;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSRcvSvrEvt
			{
				id = 22030;
			}
		}
	}
}

