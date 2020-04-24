CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2316;
	title = 231602;

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
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1414;
				taid = 2;
				type = 1;
			}
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
			cont = 231608;
			gtype = 1;
			oklnk = 2;
			area = 231601;
			goal = 231604;
			sort = 231605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 231602;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 231609;
				ctype = 1;
				idx = 4041114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 231608;
				gtype = 1;
				area = 231601;
				goal = 231604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 231605;
				stype = 1;
				taid = 1;
				title = 231602;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 1414;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 231614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 231601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1414;
			}
			CDboTSClickNPC
			{
				npcidx = "7637104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 231607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "4041114;";
			}
			CDboTSCheckClrQst
			{
				and = "2315;";
				flink = 1;
				flinknextqid = "2317;";
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 231607;
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
				iidx0 = 1414;
				taid = 1;
				type = 1;
			}
		}
	}
}

