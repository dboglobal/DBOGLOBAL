CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1012;
	title = 101202;

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
				conv = 101209;
				ctype = 1;
				idx = 5312104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 101208;
				gtype = 1;
				area = 101201;
				goal = 101204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 101205;
				stype = 2;
				taid = 1;
				title = 101202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 101207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 37;
			}
			CDboTSClickNPC
			{
				npcidx = "5312104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1013;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 101208;
			gtype = 1;
			oklnk = 2;
			area = 101201;
			goal = 101204;
			sort = 101205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 101202;
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
			desc = 101214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 101201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061301;";
			}
		}
	}
}

