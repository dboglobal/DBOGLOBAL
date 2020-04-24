CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1047;
	title = 104702;

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
				conv = 104709;
				ctype = 1;
				idx = 6312103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 3701101;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 104720;
				taid = 3;
				widx0 = 5;
				dt0 = 104721;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 104708;
				gtype = 2;
				area = 104701;
				goal = 104704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 104705;
				stype = 2;
				taid = 1;
				title = 104702;
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
			prelnk = "1;0;252;";
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
			cont = 104708;
			gtype = 2;
			oklnk = 2;
			area = 104701;
			goal = 104704;
			sort = 104705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 104702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 104714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 104701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6312103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 104707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1048;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6312103;";
			}
		}
	}
}

