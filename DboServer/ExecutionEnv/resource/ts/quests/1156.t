CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1156;
	title = 115602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 115609;
				ctype = 1;
				idx = 5063104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 115608;
				gtype = 2;
				area = 115601;
				goal = 115604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 115605;
				stype = 1;
				taid = 1;
				title = 115602;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 937;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 115620;
				taid = 3;
				widx0 = 1;
				dt0 = 115621;
				esctype = 0;
				otype0 = 7;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
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
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 115608;
			gtype = 2;
			oklnk = 2;
			area = 115601;
			goal = 115604;
			sort = 115605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 115602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 115614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 115601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 115607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1158;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5063104;";
			}
		}
	}
}

