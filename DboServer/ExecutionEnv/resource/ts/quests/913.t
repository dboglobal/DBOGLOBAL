CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 913;
	title = 91302;

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
				conv = 91309;
				ctype = 1;
				idx = 2951104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 91308;
				gtype = 1;
				area = 91301;
				goal = 91304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 91305;
				stype = 1;
				taid = 1;
				title = 91302;
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
			cont = 91308;
			gtype = 1;
			oklnk = 2;
			area = 91301;
			goal = 91304;
			sort = 91305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 91302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 91314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 91301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2951104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 91307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSCheckClrQst
			{
				and = "912;";
				flink = 1;
				flinknextqid = "915;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2951104;";
			}
		}
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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 587;
				ectype = -1;
				etype = 0;
				iidx0 = 783;
				it0 = 91320;
				taid = 1;
				widx0 = 1;
				dt0 = 91321;
				esctype = 0;
				otype0 = 7;
			}
		}
	}
}

