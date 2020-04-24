CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 549;
	title = 54902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 54907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckClrQst
			{
				and = "548;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 3411201;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 54909;
				ctype = 1;
				idx = 2611101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 54908;
				area = 54901;
				goal = 54904;
				m0fz = "-1217.000000";
				m0widx = 1;
				scitem = -1;
				sort = 54905;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "7255.000000";
				m0ttip = 54915;
				rwd = 100;
				taid = 1;
				title = 54902;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 54914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 54901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "5;";
			nolnk = 255;
			rm = 1;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 54908;
			gtype = 3;
			oklnk = 2;
			area = 54901;
			goal = 54904;
			sort = 54905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 54902;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 2;
				tgid = 1;
				time = 300000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 549;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 549;
				start = 0;
				taid = 1;
				uof = 0;
			}
			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 5;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

