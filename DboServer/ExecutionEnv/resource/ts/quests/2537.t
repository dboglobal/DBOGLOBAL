CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2537;
	title = 253702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 253709;
				ctype = 1;
				idx = 2801205;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 253708;
				gtype = 2;
				area = 253701;
				goal = 253704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 253705;
				stype = 2;
				taid = 1;
				title = 253702;
			}
			CDboTSActSToCEvt
			{
				widx1 = 14;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 253721;
				esctype = 0;
				iidx1 = -1;
				it1 = 253722;
				otype0 = 1;
				dt1 = 253723;
				etype = 0;
				it0 = 253720;
				taid = 3;
				tidx1 = 1653115;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 1655203;
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 253708;
			gtype = 2;
			oklnk = 2;
			area = 253701;
			goal = 253704;
			sort = 253705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 253702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 253707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "2801205;";
			}
			CDboTSCheckClrQst
			{
				and = "2536;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 253714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 253701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351121;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

