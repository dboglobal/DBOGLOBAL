CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1337;
	title = 133702;

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
				conv = 133709;
				ctype = 1;
				idx = 3331209;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 133708;
				gtype = 1;
				area = 133701;
				goal = 133704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 133705;
				stype = 2;
				taid = 1;
				title = 133702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 133707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "3331209;";
			}
			CDboTSCheckClrQst
			{
				and = "1336;";
				flink = 1;
				flinknextqid = "1338;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 133708;
			gtype = 1;
			oklnk = 2;
			area = 133701;
			goal = 133704;
			sort = 133705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 133702;
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
			desc = 133714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 133701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372211;";
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
	}
}

