CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 1;
	tid = 873;
	title = 87302;

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
				conv = 87309;
				ctype = 1;
				idx = 1653106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 87308;
				gtype = 1;
				area = 87301;
				goal = 87304;
				grade = 132203;
				rwd = 100;
				scitem = 99065;
				sort = 87305;
				stype = 1;
				taid = 1;
				title = 87302;
			}
			CDboTSActItem
			{
				iidx0 = 99065;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 2;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 87320;
				taid = 3;
				widx0 = 3;
				dt0 = 87321;
				esctype = 0;
				otype0 = 7;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 189;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

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
			prelnk = "100;100;";

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
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 87314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 87301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;
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
			cont = 87308;
			gtype = 1;
			oklnk = 2;
			area = 87301;
			goal = 87304;
			sort = 87305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 87302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 87307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "1653106;";
			}
			CDboTSCheckClrQst
			{
				and = "842;";
				flink = 0;
				not = 0;
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
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99065;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 87307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

