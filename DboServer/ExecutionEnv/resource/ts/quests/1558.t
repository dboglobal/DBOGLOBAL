CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1558;
	title = 155802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 943;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

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
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 943;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 155814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 155801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142216;";
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
				conv = 155809;
				ctype = 1;
				idx = 3171102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 8751201;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 155820;
				taid = 3;
				widx0 = 1;
				dt0 = 155821;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 155808;
				gtype = 1;
				area = 155801;
				goal = 155804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 155805;
				stype = 2;
				taid = 1;
				title = 155802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 155807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSCheckClrQst
			{
				and = "1541;";
				flink = 1;
				flinknextqid = "1566;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3171102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 155808;
			gtype = 1;
			oklnk = 2;
			area = 155801;
			goal = 155804;
			sort = 155805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 155802;
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

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 943;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 155807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 943;
				taid = 1;
				type = 1;
			}
		}
	}
}

