CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3025;
	title = 302502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 302507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "4511519;";
			}
			CDboTSCheckClrQst
			{
				and = "3024;";
				flink = 0;
				flinknextqid = "3026;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 302514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 302501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189107;";
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
			cont = 302508;
			gtype = 2;
			oklnk = 2;
			area = 302501;
			goal = 302504;
			sort = 302505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 302502;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 302509;
				ctype = 1;
				idx = 4511519;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 15891302;
				cnt0 = 5;
				cnt2 = 3;
				ectype = -1;
				etype = 0;
				idx1 = 15792103;
				taid = 3;
				esctype = 0;
				idx2 = 18912200;
			}
			CDboTSActRegQInfo
			{
				cont = 302508;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "3766.959961";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "3787.639893";
				m4ttip = 302517;
				area = 302501;
				goal = 302504;
				m0fz = "-5106.129883";
				m0widx = 1;
				m1ttip = 302516;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-5096.290039";
				m3ttip = 302517;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5110.509766";
				m2fx = "3968.320068";
				m2ttip = 302515;
				m4widx = 1;
				m0fx = "3752.489990";
				m0ttip = 302516;
				m3pat = 2;
				m4pat = 2;
				title = 302502;
				m1fx = "3978.760010";
				m2fz = "-5125.290039";
				sort = 302505;
				m4fz = "-5149.060059";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

