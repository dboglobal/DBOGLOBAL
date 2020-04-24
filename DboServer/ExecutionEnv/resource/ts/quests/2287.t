CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2287;
	title = 228702;

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
				conv = 228709;
				ctype = 1;
				idx = 7133104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 228708;
				gtype = 1;
				area = 228701;
				goal = 228704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 228705;
				stype = 2;
				taid = 1;
				title = 228702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 228714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 228701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7637103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 228708;
			gtype = 1;
			oklnk = 2;
			area = 228701;
			goal = 228704;
			sort = 228705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 228702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 228707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2286;";
				flink = 1;
				flinknextqid = "2288;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7133104;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

