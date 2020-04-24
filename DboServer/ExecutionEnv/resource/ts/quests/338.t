CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 1;
	rq = 0;
	tid = 338;
	title = 33802;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 5;
			}
			CDboTSActWorldPlayScript
			{
				sid = 338;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 338;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 2;
				tgid = 1;
				time = 180000;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 33809;
				ctype = 1;
				idx = 4751308;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4881105;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 33808;
				area = 33801;
				goal = 33804;
				m0fz = "-1647.199951";
				m0widx = 1;
				scitem = -1;
				sort = 33805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "5300.879883";
				m0ttip = 33815;
				rwd = 100;
				taid = 1;
				title = 33802;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 33808;
			gtype = 2;
			oklnk = 2;
			area = 33801;
			goal = 33804;
			sort = 33805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 33802;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

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
			desc = 33814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 33801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751308;";
			}
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
			stdiag = 33807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckClrQst
			{
				and = "337;";
				flink = 1;
				flinknextqid = "346;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4751308;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

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
	}
}

