CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2142;
	title = 214202;

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
				conv = 214209;
				ctype = 1;
				idx = 4153102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 214208;
				gtype = 1;
				area = 214201;
				goal = 214204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 214205;
				stype = 2;
				taid = 1;
				title = 214202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 214207;
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
				npcidx = "4153102;";
			}
			CDboTSCheckClrQst
			{
				and = "2141;";
				flink = 1;
				flinknextqid = "2143;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 214208;
			gtype = 1;
			oklnk = 2;
			area = 214201;
			goal = 214204;
			sort = 214205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 214202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 214214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 214201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2801116;";
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
	}
}

