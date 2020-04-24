CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2467;
	title = 246702;

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
				conv = 246709;
				ctype = 1;
				idx = 8031502;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 246708;
				gtype = 2;
				area = 246701;
				goal = 246704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 246705;
				stype = 2;
				taid = 1;
				title = 246702;
			}
			CDboTSActSToCEvt
			{
				widx1 = 14;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 246721;
				esctype = 0;
				iidx1 = -1;
				it1 = 246722;
				otype0 = 1;
				dt1 = 246723;
				etype = 0;
				it0 = 246720;
				taid = 3;
				tidx1 = 4072302;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 4072301;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 246714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 246701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3173112;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 246708;
			gtype = 2;
			oklnk = 2;
			area = 246701;
			goal = 246704;
			sort = 246705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 246702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 246707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "8031502;";
			}
			CDboTSCheckClrQst
			{
				and = "2466;";
				flink = 1;
				flinknextqid = "2518;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

