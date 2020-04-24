CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1112;
	title = 111202;

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
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 111209;
				ctype = 1;
				idx = 3144103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 111208;
				gtype = 1;
				area = 111201;
				goal = 111204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 111205;
				stype = 2;
				taid = 1;
				title = 111202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 111214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 111201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072107;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 111207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1113;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3144103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 111208;
			gtype = 1;
			oklnk = 2;
			area = 111201;
			goal = 111204;
			sort = 111205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 111202;
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

