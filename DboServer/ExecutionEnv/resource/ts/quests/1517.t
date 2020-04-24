CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1517;
	title = 151702;

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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 151714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 151701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7073107;";
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
				conv = 151709;
				ctype = 1;
				idx = 7073107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 151708;
				gtype = 2;
				area = 151701;
				goal = 151704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 151705;
				stype = 2;
				taid = 1;
				title = 151702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 151707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "7073107;";
			}
			CDboTSCheckClrQst
			{
				and = "1509;";
				flink = 1;
				flinknextqid = "1518;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			cont = 151708;
			gtype = 2;
			oklnk = 2;
			area = 151701;
			goal = 151704;
			sort = 151705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 151702;
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
				tidx0 = 3242201;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 151720;
				taid = 1;
				widx0 = 1;
				dt0 = 151721;
				esctype = 0;
				otype0 = 1;
			}
		}
	}
}

