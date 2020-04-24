CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2102;
	title = 210202;

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
				conv = 210209;
				ctype = 1;
				idx = 5065102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 210208;
				gtype = 1;
				area = 210201;
				goal = 210204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 210205;
				stype = 2;
				taid = 1;
				title = 210202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 210214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 210201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651210;";
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
			cont = 210208;
			gtype = 1;
			oklnk = 2;
			area = 210201;
			goal = 210204;
			sort = 210205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 210202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 210207;
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
				npcidx = "5065102;";
			}
			CDboTSCheckClrQst
			{
				and = "2101;";
				flink = 1;
				flinknextqid = "2103;";
				not = 0;
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

