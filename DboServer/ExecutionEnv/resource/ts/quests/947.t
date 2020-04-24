CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 947;
	title = 94702;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 94709;
				ctype = 1;
				idx = 3184102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 94708;
				gtype = 1;
				area = 94701;
				goal = 94704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 94705;
				stype = 1;
				taid = 1;
				title = 94702;
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
			cont = 94708;
			gtype = 1;
			oklnk = 2;
			area = 94701;
			goal = 94704;
			sort = 94705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 94702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 94707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "3184102;";
			}
			CDboTSCheckClrQst
			{
				and = "938;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 94714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 94701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3184102;";
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
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 94721;
				esctype = 0;
				iidx1 = -1;
				it1 = 94722;
				otype0 = 1;
				dt1 = 94723;
				etype = 0;
				it0 = 94720;
				taid = 1;
				tidx1 = 3172202;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3172103;
			}
		}
	}
}

