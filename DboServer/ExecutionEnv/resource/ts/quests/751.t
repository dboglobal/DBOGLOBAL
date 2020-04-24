CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 751;
	title = 75102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 751;
				start = 0;
				taid = 2;
				uof = 0;
			}
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
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 5;
				taid = 1;
				tceid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 75107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "4754203;";
			}
			CDboTSCheckClrQst
			{
				and = "714;";
				flink = 1;
				flinknextqid = "752;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 5;
				tgid = 1;
				time = 600000;
			}
			CDboTSActNPCConv
			{
				conv = 75109;
				ctype = 1;
				idx = 4754203;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 751;
				start = 1;
				taid = 4;
				uof = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 3;
				eitype = 0;
				idx0 = 5872100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5873100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 75108;
				area = 75101;
				goal = 75104;
				m0fz = "-433.000000";
				m0widx = 1;
				scitem = -1;
				sort = 75105;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "5400.000000";
				m0ttip = 75115;
				rwd = 100;
				taid = 1;
				title = 75102;
				gtype = 3;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 75114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 75101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4754203;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 75108;
			gtype = 3;
			oklnk = 2;
			area = 75101;
			goal = 75104;
			sort = 75105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 75102;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

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

