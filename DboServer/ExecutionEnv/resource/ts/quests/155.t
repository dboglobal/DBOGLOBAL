CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 155;
	title = 15502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 100;
			prelnk = "8;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 15522;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 519;
				taid = 3;
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
				taid = 2;
				esctype = 0;
				idx2 = -1;
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
				cnt1 = 0;
				eitype = 2;
				idx0 = 519;
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
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 15521;
				ctype = 1;
				idx = 4071110;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 15520;
				ctype = 1;
				idx = 8032116;
				taid = 1;
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
				cont = 15508;
				gtype = 1;
				area = 15501;
				goal = 15504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 15505;
				stype = 2;
				taid = 1;
				title = 15502;
			}
			CDboTSActNPCConv
			{
				conv = 15509;
				ctype = 1;
				idx = 4071110;
				taid = 2;
			}
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 9;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 519;
			}
			CDboTSClickNPC
			{
				npcidx = "8032116;";
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSClickNPC
			{
				npcidx = "4071110;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "8032116;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 15507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "4071110;";
			}
			CDboTSCheckClrQst
			{
				and = "154;";
				flink = 1;
				flinknextqid = "156;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 15508;
			gtype = 1;
			oklnk = 2;
			area = 15501;
			goal = 15504;
			sort = 15505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 15502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 15514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 15501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "9;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8032116;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
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
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 519;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 519;
			}
		}
	}
}

