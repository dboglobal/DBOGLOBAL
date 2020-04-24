CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 732;
	title = 73202;

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
				conv = 73209;
				ctype = 1;
				idx = 5061106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 73208;
				gtype = 1;
				area = 73201;
				goal = 73204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 73205;
				stype = 1;
				taid = 1;
				title = 73202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 73214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 73201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1111103;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 73208;
			gtype = 1;
			oklnk = 2;
			area = 73201;
			goal = 73204;
			sort = 73205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 73202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 73207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "5061106;";
			}
			CDboTSCheckClrQst
			{
				and = "729;";
				flink = 1;
				flinknextqid = "733;";
				not = 0;
			}
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
	}
}

