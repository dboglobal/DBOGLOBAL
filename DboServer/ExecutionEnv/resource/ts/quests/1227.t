CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1227;
	title = 122702;

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
				conv = 122709;
				ctype = 1;
				idx = 2412602;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 122708;
				gtype = 1;
				area = 122701;
				goal = 122704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 122705;
				stype = 2;
				taid = 1;
				title = 122702;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 5043101;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 122720;
				taid = 3;
				widx0 = 1;
				dt0 = 122721;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 122722;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 122708;
			gtype = 1;
			oklnk = 2;
			area = 122701;
			goal = 122704;
			sort = 122705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 122702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 122714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 122701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2412602;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 122707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSCheckClrQst
			{
				and = "1207;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2412602;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

