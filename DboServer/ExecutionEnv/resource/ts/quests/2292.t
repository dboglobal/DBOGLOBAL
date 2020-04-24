CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2292;
	title = 229202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 229209;
				ctype = 1;
				idx = 1491102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 229208;
				gtype = 1;
				area = 229201;
				goal = 229204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 229205;
				stype = 2;
				taid = 1;
				title = 229202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 229214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 229201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7133104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229207;
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
				npcidx = "1491102;";
			}
			CDboTSCheckClrQst
			{
				and = "2291;";
				flink = 1;
				flinknextqid = "2293;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			cont = 229208;
			gtype = 1;
			oklnk = 2;
			area = 229201;
			goal = 229204;
			sort = 229205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 229202;
		}
	}
}

