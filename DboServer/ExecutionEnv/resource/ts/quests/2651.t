CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2651;
	title = 265102;

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
				conv = 265109;
				ctype = 1;
				idx = 4192106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 265108;
				gtype = 2;
				area = 265101;
				goal = 265104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 265105;
				stype = 1;
				taid = 1;
				title = 265102;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 8411120;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 265120;
				taid = 3;
				widx0 = 1;
				dt0 = 265121;
				esctype = 0;
				otype0 = 1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 265108;
			gtype = 2;
			oklnk = 2;
			area = 265101;
			goal = 265104;
			sort = 265105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 265102;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 265114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 265101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4192106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 265107;
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
				npcidx = "4192106;";
			}
			CDboTSCheckClrQst
			{
				and = "2650;";
				flink = 1;
				flinknextqid = "2652;";
				not = 0;
			}
		}
	}
}

