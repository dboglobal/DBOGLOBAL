CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2191;
	title = 219102;

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
				conv = 219109;
				ctype = 1;
				idx = 4041113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 219108;
				gtype = 1;
				area = 219101;
				goal = 219104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 219105;
				stype = 1;
				taid = 1;
				title = 219102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 219114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 219101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111306;";
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
			cont = 219108;
			gtype = 1;
			oklnk = 2;
			area = 219101;
			goal = 219104;
			sort = 219105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 219102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 219107;
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
				and = "2190;";
				flink = 1;
				flinknextqid = "2192;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4041113;";
			}
		}
	}
}

