CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1142;
	title = 114202;

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
				conv = 114209;
				ctype = 1;
				idx = 3172201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 114208;
				gtype = 1;
				area = 114201;
				goal = 114204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 114205;
				stype = 1;
				taid = 1;
				title = 114202;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 114207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "3172201;";
			}
			CDboTSCheckClrQst
			{
				and = "1141;";
				flink = 1;
				flinknextqid = "1150;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 114208;
			gtype = 1;
			oklnk = 2;
			area = 114201;
			goal = 114204;
			sort = 114205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 114202;
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
			desc = 114214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 114201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7151128;";
			}
		}
	}
}

