CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2147;
	title = 214702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 214709;
				ctype = 1;
				idx = 4953101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 214708;
				gtype = 2;
				area = 214701;
				goal = 214704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 214705;
				stype = 2;
				taid = 1;
				title = 214702;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 3242401;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 214720;
				taid = 3;
				widx0 = 1;
				dt0 = 214721;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 214714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 214701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4953101;";
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
			cont = 214708;
			gtype = 2;
			oklnk = 2;
			area = 214701;
			goal = 214704;
			sort = 214705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 214702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 214707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "4953101;";
			}
			CDboTSCheckClrQst
			{
				and = "2146;";
				flink = 1;
				flinknextqid = "2149;";
				not = 0;
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

