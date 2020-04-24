CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11109;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 241;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 11190;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "8;";
			nolnk = 9;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 240000;
			}
			CDboTSActRegQInfo
			{
				cont = 1100847;
				m1fx = "665.849976";
				m2fz = "-269.640015";
				m1fy = "-53.490002";
				sort = 1100024;
				m2widx = 191000;
				grade = -1;
				m0fx = "717.960022";
				m0ttip = 1100848;
				m1pat = 2;
				m1widx = 191000;
				rwd = 255;
				taid = 1;
				title = 1100845;
				gtype = 1;
				area = 1100807;
				goal = 1100848;
				m0fz = "-449.869995";
				m0widx = 191000;
				m1ttip = 1100848;
				m2fy = "-59.099998";
				m2pat = 2;
				stype = 4;
				m0fy = "-55.090000";
				m0pat = 2;
				m1fz = "-331.000000";
				m2fx = "776.349976";
				m2ttip = 1100848;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7111401;
				cnt0 = 3;
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
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 11190;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
			}
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
			cid = 8;
			prelnk = "2;";
			nolnk = 7;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 11000;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "4;";
			nolnk = 5;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100847;
				m1fx = "665.849976";
				m2fz = "-269.640015";
				m1fy = "-53.490002";
				sort = 1100024;
				m2widx = 60000;
				grade = -1;
				m0fx = "717.960022";
				m0ttip = 1100848;
				m1pat = 2;
				m1widx = 60000;
				rwd = 255;
				taid = 1;
				title = 1100845;
				gtype = 1;
				area = 1100807;
				goal = 1100848;
				m0fz = "-449.869995";
				m0widx = 60000;
				m1ttip = 1100848;
				m2fy = "-59.099998";
				m2pat = 2;
				stype = 4;
				m0fy = "-55.090000";
				m0pat = 2;
				m1fz = "-331.000000";
				m2fx = "776.349976";
				m2ttip = 1100848;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 240000;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7111401;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;9;3;";
			type = 1;
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
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 11181;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "0;";
			nolnk = 240;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckInWorld
			{
				widx = 11000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "8;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 11190;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
			}
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
			cid = 2;
			prelnk = "241;5;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "4;";

			CDboTSActRegQInfo
			{
				cont = 1100847;
				m1fx = "665.849976";
				m2fz = "-269.640015";
				m1fy = "-53.490002";
				sort = 1100024;
				m2widx = 11000;
				grade = -1;
				m0fx = "717.960022";
				m0ttip = 1100848;
				m1pat = 2;
				m1widx = 11000;
				rwd = 255;
				taid = 1;
				title = 1100845;
				gtype = 1;
				area = 1100807;
				goal = 1100848;
				m0fz = "-449.869995";
				m0widx = 11000;
				m1ttip = 1100848;
				m2fy = "-59.099998";
				m2pat = 2;
				stype = 4;
				m0fy = "-55.090000";
				m0pat = 2;
				m1fz = "-331.000000";
				m2fx = "776.349976";
				m2ttip = 1100848;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 240000;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7111401;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

