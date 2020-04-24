CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 225;
	title = 22502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 704;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 704;
			}
			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 101;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 4;
				taid = 1;
				tceid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 22508;
				gtype = 1;
				area = 22501;
				goal = 22504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 22505;
				stype = 1;
				taid = 1;
				title = 22502;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 4;
				tgid = 1;
				time = 180000;
			}
			CDboTSActNPCConv
			{
				conv = 22509;
				ctype = 1;
				idx = 4651205;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92011;
				cnt1 = 0;
				eitype = 2;
				idx0 = 704;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 22514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 22501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;5;";
			usetbl = 1;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 22508;
			gtype = 1;
			oklnk = 2;
			area = 22501;
			goal = 22504;
			sort = 22505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 22502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 6;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "238;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4651205;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 704;
				taid = 1;
				type = 1;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 22507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
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

