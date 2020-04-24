CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1670;
	title = 167002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 167008;
			gtype = 2;
			oklnk = 2;
			area = 167001;
			goal = 167004;
			sort = 167005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 167002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 167007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "7134104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1671;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 167009;
				ctype = 1;
				idx = 7134104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 167008;
				gtype = 2;
				area = 167001;
				goal = 167004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 167005;
				stype = 1;
				taid = 1;
				title = 167002;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 762;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 167020;
				taid = 1;
				widx0 = 1;
				dt0 = 167021;
				esctype = 0;
				otype0 = 7;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 167014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 167001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7134104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

